using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using KeywordExtractionAPI;

namespace KEDLL
{
    class Program
    {
        static void Main(string[] args)
        {
            string TheDoc = "origin species introduction board hms beagle naturalist struck distribution organic inhabiting south america geological relations past inhabitants continent seen latter chapters volume throw light origin species mystery mysteries called philosophers return home occurred 1837 question patiently accumulating reflecting sorts possibly bearing five allowed speculate subject drew short notes enlarged 1844 sketch conclusions probable period day steadily pursued object hope excused entering personal details hasty coming decision 1859 nearly finished complete health strong urged publish abstract especially induced wallace studying natural history malay archipelago arrived exactly conclusions origin species 1858 sent memoir subject request forward sir charles lyell sent linnean society published third volume journal society sir lyell dr hooker latter read sketch 1844 honoured thinking advisable publish wallaces excellent memoir brief extracts manuscripts abstract publish necessarily imperfect references authorities statements trust reader reposing confidence accuracy doubt errors crept hope cautious trusting authorities conclusions arrived illustration hope suffice feel sensible necessity hereafter publishing detail references conclusions grounded hope future am aware scarcely single discussed volume adduced apparently leading conclusions directly opposite arrived fair result obtained stating balancing arguments question impossible regret space prevents satisfaction acknowledging generous assistance received naturalists personally unknown opportunity pass expressing deep obligations dr hooker fifteen aided stores knowledge excellent judgment considering origin species conceivable naturalist reflecting mutual affinities organic embryological relations geographical distribution geological succession conclusion species independently created descended varieties species nevertheless conclusion founded unsatisfactory shown innumerable species inhabiting world modified acquire perfection structure coadaptation justly excites admiration naturalists continually refer external conditions ";
            KeywordExtractionAPI.WORDSFRE[] wordsfre;

            wordsfre = KeywordExtractionAPI.MyFun.StatisticsWords(TheDoc);
            foreach (KeywordExtractionAPI.WORDSFRE wf in wordsfre)
            {
                wf.EntropyDifference_Normal();
            }
            KeywordExtractionAPI.MyFun.QuickSort(wordsfre, 0, wordsfre.Length - 1);

            Console.WriteLine("TheDoc:\n" + TheDoc);
            Console.WriteLine("\nwordsfre:");
            foreach (KeywordExtractionAPI.WORDSFRE wf in wordsfre)
            {
                Console.WriteLine(wf.Word + "\t" + wf.ED);
            }
            Console.ReadKey();
        }
    }
}
